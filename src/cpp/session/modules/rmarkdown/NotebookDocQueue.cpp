/*
 * NotebookDocQueue.cpp
 *
 * Copyright (C) 2009-16 by RStudio, Inc.
 *
 * Unless you have received this program directly from RStudio pursuant
 * to the terms of a commercial license agreement with RStudio, then
 * this program is licensed to you under the terms of version 3 of the
 * GNU Affero General Public License. This program is distributed WITHOUT
 * ANY EXPRESS OR IMPLIED WARRANTY, INCLUDING THOSE OF NON-INFRINGEMENT,
 * MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE. Please refer to the
 * AGPL (http://www.gnu.org/licenses/agpl-3.0.txt) for more details.
 *
 */

#include "NotebookDocQueue.hpp"

#include <boost/foreach.hpp>

#include <session/SessionModuleContext.hpp>

using namespace rstudio::core;

#define kDocQueueId      "doc_id"
#define kDocQueueJobDesc "job_desc"
#define kDocQueueUnits   "units"

namespace rstudio {
namespace session {
namespace modules {
namespace rmarkdown {
namespace notebook {

NotebookDocQueue::NotebookDocQueue(const std::string& docId, 
      const std::string& jobDesc, int pixelWidth, int charWidth) :
      docId_(docId),
      jobDesc_(jobDesc),
      pixelWidth_(pixelWidth),
      charWidth_(charWidth)
{
}

boost::shared_ptr<NotebookQueueUnit> NotebookDocQueue::firstUnit()
{
   if (queue_.empty())
      return boost::shared_ptr<NotebookQueueUnit>();
   return *queue_.begin();
}

json::Object NotebookDocQueue::toJson() const
{
   // serialize all the queue units 
   json::Array units;
   BOOST_FOREACH(const boost::shared_ptr<NotebookQueueUnit> unit, queue_) 
   {
      units.push_back(unit->toJson());
   }

   // form JSON object for client
   json::Object queue;
   queue[kDocQueueId]      = docId_;
   queue[kDocQueueJobDesc] = jobDesc_;
   queue[kDocQueueUnits]   = units;

   return queue;
}

Error NotebookDocQueue::update(const boost::shared_ptr<NotebookQueueUnit> unit, 
      QueueOperation op, const std::string& before)
{
   std::list<boost::shared_ptr<NotebookQueueUnit> >::iterator it;

   switch(op)
   {
      case QueueAdd:
         // find insertion position
         for (it = queue_.begin(); it != queue_.end(); it++)
         {
            if ((*it)->chunkId() == before)
               break;
         }
         queue_.insert(it, unit);
         break;

      case QueueUpdate:
         break;

      case QueueDelete:
         break;
   }
   return Success();
}

std::string NotebookDocQueue::docId() const
{
   return docId_;
}

} // namespace notebook
} // namespace rmarkdown
} // namespace modules
} // namespace session
} // namespace rstudio
